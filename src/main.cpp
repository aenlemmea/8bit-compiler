#include "codegen/codegenarator.hh"
#include <iostream>

std::string /* Copy Elision, No worries */ read_input(std::ifstream& inp) {
    std::string owner { std::istreambuf_iterator<char>(inp),  std::istreambuf_iterator<char>() };
    return owner;
}

auto main(int32_t argc, char** argv) -> int
{
    using namespace etbit;

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename> " << "--show-parsed\n";
        exit(EXIT_FAILURE);
    }
    std::ifstream inp(argv[1]);
    bool show_parsed = (argv[2] == nullptr ? false : true);

    if (!inp.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string input = read_input(inp);

    auto filename = [&argv]() -> std::string {
        std::string name(argv[1]);
        if (auto temp = name.find("."); temp != std::string::npos) {
            return name.substr(0, temp);
        }
        return name;
    };

    lexer::lexer lex{ input };
    parser::parser prs{lex};
    codegen::generator gen{ prs, std::cout};
    gen.generate_to_file(filename() + ".asm");
    if (show_parsed) {
        gen.program.print(std::cout);
    }
}
