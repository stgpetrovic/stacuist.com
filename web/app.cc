#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <glog/logging.h>

#include "absl/strings/str_cat.h"
#include "engine/recipe.h"
#include "web/tags.h"

namespace stacuist::web {

class StaCuIstApplication : public Wt::WApplication {
 public:
  StaCuIstApplication(const Wt::WEnvironment &env,
                      std::unique_ptr<Wt::Dbo::Session> session);

 private:
  Wt::WText *recipe_text_;
  std::unique_ptr<Wt::Dbo::Session> session_;
};

StaCuIstApplication::StaCuIstApplication(
    const Wt::WEnvironment &env, std::unique_ptr<Wt::Dbo::Session> session)
    : Wt::WApplication(env), session_(std::move(session)) {
  setTitle("Šta ću ist?!");

  {
    Wt::Dbo::Transaction transaction{*session_};
    Wt::Dbo::collection<Wt::Dbo::ptr<engine::Tag>> tags =
        session_->find<engine::Tag>();
    root()->addWidget(std::make_unique<TagsWidget>(tags));
  }

  root()->addWidget(std::make_unique<Wt::WText>("Sta bi jia?"));
  Wt::WPushButton *button =
      root()->addWidget(std::make_unique<Wt::WPushButton>("Nadji"));
  root()->addWidget(std::make_unique<Wt::WBreak>());

  recipe_text_ = root()->addWidget(std::make_unique<Wt::WText>());
}

};  // namespace stacuist::web

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);

  return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
    std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3{
        new Wt::Dbo::backend::Sqlite3("/home/slon/recipes.db")};
    auto session = std::make_unique<Wt::Dbo::Session>();
    session->setConnection(std::move(sqlite3));

    session->mapClass<stacuist::engine::Tag>("tag");
    session->mapClass<stacuist::engine::Recipe>("recipe");

    return std::make_unique<stacuist::web::StaCuIstApplication>(
        env, std::move(session));
  });
}

