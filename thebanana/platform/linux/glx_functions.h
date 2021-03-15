#pragma once
struct glx_functions_t {
    using CHOOSEFBCONFIG = GLXFBConfig*(*)(Display*,int,const int*,int*);
    using GETFBCONFIGATTRIB = int(*)(Display*,GLXFBConfig,int,int*);
    using CREATEWINDOW = GLXWindow(*)(Display*,GLXFBConfig,Window,const int*);
    using CREATENEWCONTEXT = GLXContext(*)(Display*,GLXFBConfig,int,GLXContext,Bool);
    using MAKECONTEXTCURRENT = Bool(*)(Display*,GLXDrawable,GLXDrawable,GLXContext);
    CHOOSEFBCONFIG _glXChooseFBConfig;
    GETFBCONFIGATTRIB _glXGetFBConfigAttrib;
    CREATEWINDOW _glXCreateWindow;
    CREATENEWCONTEXT _glXCreateNewContext;
    MAKECONTEXTCURRENT _glXMakeContextCurrent;
    void* opengl_lib;
    ~glx_functions_t();
};
extern glx_functions_t glx_functions;