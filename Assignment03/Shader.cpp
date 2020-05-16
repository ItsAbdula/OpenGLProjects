//#include <string>
//#include "Shader.h"
//
//class VertexShader : Shader
//{
//public:
//    VertexShader()
//    {
//        ID = glCreateShader(GL_VERTEX_SHADER);
//    }
//
//    VertexShader(const GLchar *src)
//    {
//        ID = glCreateShader(GL_VERTEX_SHADER);
//        glShaderSource(ID, 1, const &src, NULL);
//        glCompileShader(vertexShader);
//    }
//
//    GLuint getID() const
//    {
//        return ID;
//    }
//
//private:
//    GLuint ID;
//};
//
//GLuint Shader::getID() const
//{
//    return GLuint();
//}