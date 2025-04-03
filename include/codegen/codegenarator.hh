#ifndef ETBIT_CODEGEN_HH
#define ETBIT_CODEGEN_HH

#include <fstream>

#include "../front/parse/ast.hh"
#include "../front/parse/parser.hh"

// This is a supporting class only for code gen helper functions and as such all generation logic is done in parse/ast.hh

namespace etbit::codegen
{
    struct generator {
        generator(parser::parser& _prs, std::ostream& _os) : prs(_prs) {
            program = _prs.parse_context();
            if (_prs.get_error_count() > 0) {
                for (const auto& e : prs.errors) {
                    _os << e << "\n";
                }
                _os << "Found errors. Exiting\n";
                std::exit(EXIT_SUCCESS);
            }
        }

        void generate_to_file(const std::string& filename_with_extension) {
            std::ofstream out;
            out.open(filename_with_extension);
            out << ".text\n\n";
            program.generate(out);
            out << "\n\nhlt\n";
            out.close(); // RAII gone? jk
        }

        parser::parser& prs;
        ast::context program;
    };
};

#endif
