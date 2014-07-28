//
//  Shader.fsh
//  qaz
//
//  Created by ios on 14-7-26.
//  Copyright (c) 2014年 abc. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
