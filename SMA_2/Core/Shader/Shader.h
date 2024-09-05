#include <string>

struct ShaderSource {
    std::string vertexSource;
    std::string fragmentSource;
};

namespace Shader {
    extern unsigned int Program;
    ShaderSource ReadShaderFromFile(const char* filepathVert, const char* filepathFrag);
    void CreateProgram();   
};
