/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef __DataStream_H__
#define __DataStream_H__

#include <istream>
#include <System/Misc/StrUtil.h>

namespace Ogre {
	/** General purpose class used for encapsulating the reading of data.
	@remarks
		This class performs basically the same tasks as std::basic_istream, 
		except that it does not have any formatting capabilities, and is
		designed to be subclassed to receive data from multiple sources,
		including libraries which have no compatiblity with the STL's
		stream interfaces. As such, this is an abstraction of a set of 
		wrapper classes which pretend to be standard stream classes but 
		can actually be implemented quite differently. 
	@par
		Generally, if a plugin or application provides an ArchiveFactory, 
		it should also provide a DataStream subclass which will be used
		to stream data out of that Archive implementation, unless it can 
		use one of the common implementations included.
	@note
		Ogre makes no guarantees about thread safety, for performance reasons.
		If you wish to access stream data asynchronously then you should
		organise your own mutexes to avoid race conditions. 
	*/
	class DataStream
	{
	protected:
		/// The name (e.g. resource name) that can be used to identify the source fot his data (optional)
		std::string mName;		
        /// Size of the data in the stream (may be 0 if size cannot be determined)
        size_t mSize;
		EnumEncoding m_encoding;

        #define OGRE_STREAM_TEMP_SIZE 128
	public:
		/// Constructor for creating unnamed streams
        DataStream() : mSize(0),m_encoding(encode_ansii) {}
		/// Constructor for creating named streams
		DataStream(const std::string& name) : mName(name), mSize(0),m_encoding(encode_ansii) {}

		/// Returns the name of the stream, if it has one.
		const char* getName(void) { return mName.c_str(); }
		void setName(const std::string& strName){mName = strName;}

        virtual ~DataStream() {}
		// Streaming operators
        template<typename T> DataStream& operator>>(T& val);
		/** Read the requisite number of bytes from the stream, 
			stopping at the end of the file.
		@param buf Reference to a buffer pointer
		@param count Number of bytes to read
		@returns The number of bytes read
		*/
		virtual size_t read(void* buf, size_t count) = 0;
		/** Get a single line from the stream.
		@remarks
			The delimiter character is not included in the data
			returned, and it is skipped over so the next read will occur
			after it. The buffer contents will include a
			terminating character.
        @note
            If you used this function, you <b>must</b> open the stream in <b>binary mode</b>,
            otherwise, it'll produce unexpected results.
		@param buf Reference to a buffer pointer
		@param maxCount The maximum length of data to be read, excluding the terminating character
		@param delim The delimiter to stop at
		@returns The number of bytes read, excluding the terminating character
		*/
		virtual size_t readLine(char* buf, size_t maxCount, const std::string& delim = "\n");
		
	    /** Returns a String containing the next line of data, optionally 
		    trimmed for whitespace. 
	    @remarks
		    This is a convenience method for text streams only, allowing you to 
		    retrieve a String object containing the next line of data. The data
		    is read up to the next newline character and the result trimmed if
		    required.
        @note
            If you used this function, you <b>must</b> open the stream in <b>binary mode</b>,
            otherwise, it'll produce unexpected results.
	    @param 
		    trimAfter If true, the line is trimmed for whitespace (as in 
		    String.trim(true,true))
	    */
	    virtual std::string getLine( bool trimAfter = true );

	    /** Returns a String containing the entire stream. 
	    @remarks
		    This is a convenience method for text streams only, allowing you to 
		    retrieve a String object containing all the data in the stream.
	    */
	    virtual std::string getAsString(void);

		/** Skip a single line from the stream.
        @note
            If you used this function, you <b>must</b> open the stream in <b>binary mode</b>,
            otherwise, it'll produce unexpected results.
		@param delim The delimiter(s) to stop at
		@returns The number of bytes skipped
		*/
		virtual size_t skipLine(const std::string& delim = "\n");

		/** Skip a defined number of bytes. This can also be a negative value, in which case
		the file pointer rewinds a defined number of bytes. */
		virtual void skip(long count) = 0;
	
		/** Repositions the read point to a specified byte.
	    */
	    virtual void seek( size_t pos ) = 0;
		
		/** Returns the current byte offset from beginning */
	    virtual size_t tell(void) const = 0;

		/** Returns true if the stream has reached the end.
	    */
	    virtual bool eof(void) const = 0;

		/** Returns the total size of the data to be read from the stream, 
			or 0 if this is indeterminate for this stream. 
		*/
        size_t size(void) const { return mSize; }

        /** Close the stream; this makes further operations invalid. */
        virtual void close(void) = 0;
		/** Get/Set the data of this stream by the pos.
		*/
		virtual size_t add16BitData( size_t pos, unsigned short val) { return 0; }
		virtual size_t add32BitData( size_t pos, unsigned int val) { return 0; }
		virtual unsigned char* getData(size_t pos) { return NULL; }
		virtual void rewind(void) {}

	};

	/** Shared pointer to allow data streams to be passed around without
		worrying about deallocation
	*/



    /** Common subclass of DataStream for handling data from 
		std::basic_istream.
	*/
	class FileStreamDataStream : public DataStream
	{
	protected:
		/// Reference to source stream
		std::ifstream* mpStream;
        bool mFreeOnClose;			
		bool mbCloseFile;
	public:
		/** Construct stream from an STL stream
        @param s Pointer to source stream
        @param freeOnClose Whether to delete the underlying stream on 
            destruction of this class
        */
		FileStreamDataStream(std::ifstream* s, 
            bool freeOnClose = true);
		/** Construct named stream from an STL stream
        @param name The name to give this stream
        @param s Pointer to source stream
        @param freeOnClose Whether to delete the underlying stream on 
            destruction of this class
        */
		FileStreamDataStream(const std::string& name, 
            std::ifstream* s, 
            bool freeOnClose = true);

		/** Construct named stream from an STL stream, and tell it the size
        @remarks
            This variant tells the class the size of the stream too, which 
            means this class does not need to seek to the end of the stream 
            to determine the size up-front. This can be beneficial if you have
            metadata about the contents of the stream already.
        @param name The name to give this stream
        @param s Pointer to source stream
        @param size Size of the stream contents in bytes
        @param freeOnClose Whether to delete the underlying stream on 
            destruction of this class
        */
		FileStreamDataStream(const std::string& name, 
            std::ifstream* s, 
            size_t size, 
            bool freeOnClose = true);

        ~FileStreamDataStream();

		/** @copydoc DataStream::read
		*/
		size_t read(void* buf, size_t count);
		/** @copydoc DataStream::readLine
		*/
        size_t readLine(char* buf, size_t maxCount, const std::string& delim = "\n");
		
		/** @copydoc DataStream::skip
		*/
		void skip(long count);
	
		/** @copydoc DataStream::seek
		*/
	    void seek( size_t pos );

		/** @copydoc DataStream::tell
		*/
		size_t tell(void) const;

		/** @copydoc DataStream::eof
		*/
	    bool eof(void) const;

        /** @copydoc DataStream::close
        */
        void close(void);

		void setFileClose(bool bClose){mbCloseFile = bClose;}
		void setFreeOnClose(bool bfree){mFreeOnClose = bfree;}
		std::ifstream* getFileStream(){return mpStream;}
		
	};

}
	

#endif

