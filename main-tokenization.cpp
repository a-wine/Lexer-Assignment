#include "tokenization.cpp"

int main() {
    std::string source = R"(
        let x = 534;
        let y = 10;
        print x + y;
    )";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    for (const auto &token : tokens) {
        std::cout << "Token: " << token.lexeme
                  << " Line: " << token.line
                  << " Type: " << static_cast<int>(token.type) << "\n";
    }

    return 0;
}
