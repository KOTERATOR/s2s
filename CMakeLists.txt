cmake_minimum_required(VERSION 3.20)
project(s2s)

set(CMAKE_CXX_STANDARD 14)

add_executable(s2s main.cpp s2s/S2S.cpp s2s/S2S.h s2s/Lexer/Lexer.cpp s2s/Lexer/Lexer.h s2s/Parser/Parser.cpp s2s/Parser/Parser.h s2s/Runtime/Runtime.cpp s2s/Runtime/Runtime.h s2s/Types/Type.h s2s/Types/ArrayType.cpp s2s/Types/ArrayType.h s2s/Types/FloatType.cpp s2s/Types/FloatType.h s2s/Types/IntType.cpp s2s/Types/IntType.h s2s/Types/StringType.cpp s2s/Types/StringType.h s2s/Lexer/LexerException.cpp s2s/Lexer/LexerException.h s2s/Lexer/Token.cpp s2s/Lexer/Token.h s2s/Lexer/LexerLine.cpp s2s/Lexer/LexerLine.h s2s/Types/BoolType.cpp s2s/Types/BoolType.h s2s/Types/Function.cpp s2s/Types/Function.h s2s/Runtime/RuntimeException.cpp s2s/Runtime/RuntimeException.h s2s/Runtime/Block.cpp s2s/Runtime/Block.h s2s/Builtin/Functions/Print.cpp s2s/Builtin/Functions/Print.h s2s/Builtin/Functions/Types.h s2s/Types/AllTypes.h s2s/Builtin/Functions/Input.h s2s/Parser/LineReader.h s2s/Parser/LinesReader.h s2s/Parser/Reader.h)
