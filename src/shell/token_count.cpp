#include <fstream>
#include <iostream>
#include "frontends/lean/scanner.h"
#include "kernel/environment.h"
#include "library/io_state.h"
#include "init/init.h"
#include "kernel/standard_kernel.h"
#include "frontends/lean/token_table.h"
#include "frontends/lean/parser_config.h"

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cout << "Missing argument file" << std::endl;
    return 1;
  }

  std::filebuf fb;
  std::string path(argv[1]);
  if (!fb.open(path, std::ios::in)) {
    std::cout << "Fail to open " << argv[1] << std::endl;
    return 1;
  }
  std::istream istr(&fb);

  std::vector<std::string> new_tokens = {"∨", "∃", "↦", "*", "=", "ℕ", "∧", ";", "<|>"};
  lean::initialize();
  lean::environment env = lean::mk_environment();
  for (std::string& tok : new_tokens) {
    env = lean::add_token(env, lean::token_entry(tok));
  }

  lean::scanner scanner(istr);
  lean::buffer<lean::token> buffer;
  int count = 0;
  while (true) {
    lean::token tok = lean::read_tokens(
                                        env,
                                        lean::get_global_ios(),
                                        scanner,
                                        buffer,
                                        true);
    if (tok.kind() == lean::token_kind::Eof) {
      break;
    } else if (tok.kind() == lean::token_kind::CommandKeyword) {
      count += 1;
      // std::cout << tok.get_token_info().token() << std::endl;
    }
  }

  count += buffer.size();
  std::cout << count << std::endl;

  return 0;
}
