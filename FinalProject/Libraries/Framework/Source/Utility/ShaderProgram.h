#ifndef __ShaderBase_H__
#define __ShaderBase_H__

class ShaderProgram
{
protected:
    char* m_VertShaderString;
    char* m_FragShaderString;
    bool m_StringsWereAllocated;

    GLuint m_VertShader;
    GLuint m_FragShader;
    GLuint m_Program;

public:
    ShaderProgram();
    ShaderProgram(const char* vertfilename, const char* fragfilename);
    virtual ~ShaderProgram();

    void Cleanup();

    GLuint CompileShader(GLenum shaderType, const char* shaderstring);
    bool Init(const char* vertfilename, const char* fragfilename);
    bool InitFromStrings(char* vertstring, char* fragstring);

    GLuint GetProgram() { return m_Program; }
};

#endif //__ShaderBase_H__
