#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/WApplication.h>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "engine/recipe.h"
#include "web/app.h"

ABSL_FLAG(std::string, db_path, "/root/stacuist.com/recipes.db",
          "path to the sqlite3 database");

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
    std::cerr << "Connecting to " << absl::GetFlag(FLAGS_db_path) << std::endl;
    std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3{
        new Wt::Dbo::backend::Sqlite3(absl::GetFlag(FLAGS_db_path))};
    auto session = std::make_unique<Wt::Dbo::Session>();
    session->setConnection(std::move(sqlite3));

    session->mapClass<stacuist::engine::Tag>("tag");
    session->mapClass<stacuist::engine::Recipe>("recipe");

    return std::make_unique<stacuist::web::StaCuIstApplication>(
        env, std::move(session));
  });
}

