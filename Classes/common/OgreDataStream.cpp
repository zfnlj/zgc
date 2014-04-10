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
#include "OgreDataStream.h"
#include <iostream>
#include <fstream>
#include "../Inc/KTypeDef.h"
#ifdef _USE_COCOS2DX
#include "cocos2d.h"
#endif
namespace Ogre {
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    template <typename T> DataStream& DataStream::operator >>(T& val)
    {
        read(static_cast<void*>(&val), sizeof(T));
        return *this;
    }
    //-----------------------------------------------------------------------
    std::string DataStream::getLine(bool trimAfter)
    {
		size_t currpos = this->tell();

        char tmpBuf[OGRE_STREAM_TEMP_SIZE];
        std::string retString;
        size_t readCount;

		// Keep looping while not hitting delimiter
        while ((readCount = read(tmpBuf, OGRE_STREAM_TEMP_SIZE-1)) != 0)
        {
			// Terminate string
            tmpBuf[readCount] = '\0';

			char* line = tmpBuf;
			if(!currpos && readCount >= 3)
			{
				currpos += readCount;
				int flag = 0; memcpy(&flag, line, 3);
				if(flag == 0xbfbbef)
				{
					line += 3;
					readCount -= 3;
					m_encoding = encode_utf8;
				}
			}
			else
			{
				currpos += readCount;
			}

            char* p = strchr(line, '\n');
            if (p != 0)
            {
                // Reposition backwards
                skip((long)(p + 1 - line - readCount));
                *p = '\0';
            }
			
            retString += line;

            if (p != 0)
            {
                // Trim off trailing CR if this was a CR/LF entry
                if (retString.length() && retString[retString.length()-1] == '\r')
                {
                    retString.erase(retString.length()-1, 1);
                }

                // Found terminator, break out
                break;
            }
        }

        if (trimAfter)
        {
			CCAssert(false , "StringUtil::trim(retString)!");
            //StringUtil::trim(retString);
        }

		if(m_encoding != g_encoding)
		{
			int nn; char buf2[4096]; ASSERT(retString.size() < sizeof(buf2));
			if(m_encoding == encode_utf8) nn = utf8_to_local(retString.c_str(), retString.size(), buf2, sizeof(buf2));
			else nn = local_to_utf8(retString.c_str(), retString.size(), buf2, sizeof(buf2));
			if(nn < 0) return "";
			retString.assign(buf2, nn);
			return retString;
		}
		return retString;
    }
    //-----------------------------------------------------------------------
    size_t DataStream::readLine(char* buf, size_t maxCount, const std::string& delim)
    {
		// Deal with both Unix & Windows LFs
		bool trimCR = false;
		if (delim.find_first_of('\n') != std::string::npos)
		{
			trimCR = true;
		}

		size_t currpos = this->tell();
        char tmpBuf[OGRE_STREAM_TEMP_SIZE];
        size_t chunkSize = std::min(maxCount, (size_t)OGRE_STREAM_TEMP_SIZE-1);
        size_t totalCount = 0;
        size_t readCount; 
        while (chunkSize && (readCount = read(tmpBuf, chunkSize)))
        {
            // Terminate
            tmpBuf[readCount] = '\0';

			char* line = tmpBuf;
			if(!currpos && readCount >= 3)
			{
				currpos += readCount;
				int flag = 0; memcpy(&flag, line, 3);
				if(flag == 0xbfbbef)
				{
					line += 3;
					readCount -= 3;
					m_encoding = encode_utf8;
				}
			}
			else
			{
				currpos += readCount;
			}

            // Find first delimiter
            size_t pos = strcspn(line, delim.c_str());

            if (pos < readCount)
            {
                // Found terminator, reposition backwards
                skip((long)(pos + 1 - readCount));
            }

            // Are we genuinely copying?
            if (buf)
            {
                memcpy(buf+totalCount, line, pos);
            }
            totalCount += pos;

            if (pos < readCount)
            {
                // Trim off trailing CR if this was a CR/LF entry
                if (trimCR && totalCount && buf[totalCount-1] == '\r')
                {
                    --totalCount;
                }

                // Found terminator, break out
                break;
            }

            // Adjust chunkSize for next time
            chunkSize = std::min(maxCount-totalCount, (size_t)OGRE_STREAM_TEMP_SIZE-1);
        }

        // Terminate
        buf[totalCount] = '\0';
		if(m_encoding != g_encoding)
		{
			int bytes = 0;
			char buf2[4096]; ASSERT(totalCount < sizeof(buf2)); memcpy(buf2, buf, totalCount);
			if(m_encoding == encode_utf8) bytes = utf8_to_local(buf2, totalCount, buf, maxCount);
			else bytes = local_to_utf8(buf2, totalCount, buf, maxCount);
			if(bytes < 0) return 0;
			return bytes;
		}
        return totalCount;
    }
    //-----------------------------------------------------------------------
    size_t DataStream::skipLine(const std::string& delim)
    {
        char tmpBuf[OGRE_STREAM_TEMP_SIZE];
        size_t total = 0;
        size_t readCount;
		size_t currpos = this->tell();
        // Keep looping while not hitting delimiter
        while ((readCount = read(tmpBuf, OGRE_STREAM_TEMP_SIZE-1)) != 0)
        {
            // Terminate string
            tmpBuf[readCount] = '\0';

			char* line = tmpBuf;
			if(!currpos && readCount >= 3)
			{
				currpos += readCount;
				int flag = 0; memcpy(&flag, line, 3);
				if(flag == 0xbfbbef)
				{
					line += 3;
					readCount -= 3;
					m_encoding = encode_utf8;
				}
			}
			else
			{
				currpos += readCount;
			}

            // Find first delimiter
            size_t pos = strcspn(line, delim.c_str());

            if (pos < readCount)
            {
                // Found terminator, reposition backwards
                skip((long)(pos + 1 - readCount));

                total += pos + 1;

                // break out
                break;
            }

            total += readCount;
        }

        return total;
    }
    //-----------------------------------------------------------------------
    std::string DataStream::getAsString(void)
    {
        // Read the entire buffer
        char* pBuf = new char[mSize+1];
        // Ensure read from begin of stream
        seek(0);
        read(pBuf, mSize);
        pBuf[mSize] = '\0';
        std::string str;
        str.insert(0, pBuf, mSize);
        delete [] pBuf;
        return str;
    }
    //-----------------------------------------------------------------------
    FileStreamDataStream::FileStreamDataStream(std::ifstream* s, bool freeOnClose)
        : DataStream(), mpStream(s), mFreeOnClose(freeOnClose),mbCloseFile(true)
    {
        // calculate the size
		s->seekg(0, std::ios_base::end);
        mpStream->seekg(0, std::ios_base::end);
        mSize = mpStream->tellg();
        mpStream->seekg(0, std::ios_base::beg);

    }
    //-----------------------------------------------------------------------
    FileStreamDataStream::FileStreamDataStream(const std::string& name, 
        std::ifstream* s, bool freeOnClose)
        : DataStream(name), mpStream(s), mFreeOnClose(freeOnClose),mbCloseFile(true)
    {
        // calculate the size
        mpStream->seekg(0, std::ios_base::end);
        mSize = mpStream->tellg();
        mpStream->seekg(0, std::ios_base::beg);
    }
    //-----------------------------------------------------------------------
    FileStreamDataStream::FileStreamDataStream(const std::string& name, 
        std::ifstream* s, size_t size, bool freeOnClose)
        : DataStream(name), mpStream(s), mFreeOnClose(freeOnClose),mbCloseFile(true)
    {
        // Size is passed in
        mSize = size;
    }
    //-----------------------------------------------------------------------
    FileStreamDataStream::~FileStreamDataStream()
    {
        close();
    }
    //-----------------------------------------------------------------------
    size_t FileStreamDataStream::read(void* buf, size_t count)
    {
		mpStream->read(static_cast<char*>(buf), static_cast<std::streamsize>(count));
        return mpStream->gcount();
    }
    //-----------------------------------------------------------------------
    size_t FileStreamDataStream::readLine(char* buf, size_t maxCount, 
        const std::string& delim)
    {
		if (delim.empty())
		{
			CCAssert(false , "No delimiter provided!");
		}
		if (delim.size() > 1)
		{
			CCAssert(false , "WARNING: FileStreamDataStream::readLine - using only first delimeter!");
		}
		// Deal with both Unix & Windows LFs
		bool trimCR = false;
		if (delim.at(0) == '\n') 
		{
			trimCR = true;
		}

		if(!this->tell())
		{
			int flag = 0;
			if(this->read(&flag, 3) == 3 && flag == 0xbfbbef)
			{
				m_encoding = encode_utf8;
			}
			else
			{
				this->seek(0);
			}
		}

		// maxCount + 1 since count excludes terminator in getline
		mpStream->getline(buf, static_cast<std::streamsize>(maxCount+1), delim.at(0));
		size_t ret = mpStream->gcount();
		// three options
		// 1) we had an eof before we read a whole line
		// 2) we ran out of buffer space
		// 3) we read a whole line - in this case the delim character is taken from the stream but not written in the buffer so the read data is of length ret-1 and thus ends at index ret-2
		// in all cases the buffer will be null terminated for us

		if (mpStream->eof()) 
		{
			// no problem
		}
		else if (mpStream->fail())
		{
			// Did we fail because of maxCount hit? No - no terminating character
			// in included in the count in this case
			if (ret == maxCount)
			{
				// clear failbit for next time 
				mpStream->clear();
			}
			else
			{
				CCAssert(false , "Streaming error occurred!");
			}
		}
		else 
		{
			// we need to adjust ret because we want to use it as a
			// pointer to the terminating null character and it is
			// currently the length of the data read from the stream
			// i.e. 1 more than the length of the data in the buffer and
			// hence 1 more than the _index_ of the NULL character
			--ret;
		}

		// trim off CR if we found CR/LF
		if (trimCR && buf[ret-1] == '\r')
		{
			--ret;
			buf[ret] = '\0';
		}
		if(m_encoding != g_encoding)
		{
			int bytes = 0; char buf2[4096]; ASSERT(ret < sizeof(buf2)); memcpy(buf2, buf, ret);
			if(m_encoding == encode_utf8) bytes = utf8_to_local(buf2, ret, buf, maxCount);
			else bytes = local_to_utf8(buf2, ret, buf, maxCount);
			if(bytes < 0) return 0;
			return bytes;
		}
		return ret;
	}
    //-----------------------------------------------------------------------
    void FileStreamDataStream::skip(long count)
    {
#if defined(STLPORT)
		// Workaround for STLport issues: After reached eof of file stream,
		// it's seems the stream was putted in intermediate state, and will be
		// fail if try to repositioning relative to current position.
		// Note: tellg() fail in this case too.
		if (mpStream->eof())
		{
			mpStream->clear();
			// Use seek relative to either begin or end to bring the stream
			// back to normal state.
			mpStream->seekg(0, std::ios::end);
		}
#endif 		
		mpStream->clear(); //Clear fail status in case eof was set
		mpStream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
    }
    //-----------------------------------------------------------------------
    void FileStreamDataStream::seek( size_t pos )
    {
		mpStream->clear(); //Clear fail status in case eof was set
        mpStream->seekg(static_cast<std::streamoff>(pos), std::ios::beg);
    }
	//-----------------------------------------------------------------------
    size_t FileStreamDataStream::tell(void) const
	{
		mpStream->clear(); //Clear fail status in case eof was set
		return mpStream->tellg();
	}
	//-----------------------------------------------------------------------
    bool FileStreamDataStream::eof(void) const
    {
        return mpStream->eof();
    }
    //-----------------------------------------------------------------------
    void FileStreamDataStream::close(void)
    {
        if (mpStream)
        {
			if (mbCloseFile)
			{
				mpStream->close();
	
				if (mFreeOnClose)
				{
					// delete the stream too
					delete mpStream;
					mpStream = 0;
				}
			}
        }
    }
}