#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

#include "absl/strings/str_cat.h"
#include "engine/recipe.h"
#include "web/tags.h"

namespace stacuist::web {

class StaCuIstApplication : public Wt::WApplication {
 public:
  StaCuIstApplication(const Wt::WEnvironment &env,
                      std::unique_ptr<Wt::Dbo::Session> session);

 private:
  Wt::WLineEdit *nameEdit_;
  Wt::WText *greeting_;
  std::unique_ptr<Wt::Dbo::Session> session_;
};

StaCuIstApplication::StaCuIstApplication(
    const Wt::WEnvironment &env, std::unique_ptr<Wt::Dbo::Session> session)
    : Wt::WApplication(env), session_(std::move(session)) {
  setTitle("Šta ću ist?!");

  root()->addWidget(std::make_unique<Wt::WText>("Sta bi jia?"));
  nameEdit_ = root()->addWidget(std::make_unique<Wt::WLineEdit>());
  Wt::WPushButton *button =
      root()->addWidget(std::make_unique<Wt::WPushButton>("Nadji"));
  root()->addWidget(std::make_unique<Wt::WBreak>());

  {
    Wt::Dbo::Transaction transaction{*session_};
    Wt::Dbo::collection<Wt::Dbo::ptr<engine::Tag>> tags =
        session_->find<engine::Tag>();
    std::cout << "ZAJSSSSSS tag " << tags.size();
    root()->addWidget(std::make_unique<TagsWidget>(tags));
  }

  greeting_ = root()->addWidget(std::make_unique<Wt::WText>());

  auto greet = [this] {
    std::string text = "nema taj riceta";
    Wt::Dbo::Transaction transaction{*session_};
    typedef Wt::Dbo::collection<Wt::Dbo::ptr<engine::Recipe>> Recipes;
    Recipes recipes = session_->find<engine::Recipe>()
                          .where("name = ?")
                          .bind(nameEdit_->text().toUTF8());
    if (recipes.size() > 0) {
      for (const auto &recipe : recipes) {
        text = absl::StrCat(recipe->name, "(", recipe->author,
                            "): ", recipe->text);
        break;
      }
    }
    greeting_->setText(text);
  };
  button->clicked().connect(greet);
}
};  // namespace stacuist::web

int main(int argc, char **argv) {
  return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
    std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3{
        new Wt::Dbo::backend::Sqlite3("/home/slon/recipes.db")};
    auto session = std::make_unique<Wt::Dbo::Session>();
    session->setConnection(std::move(sqlite3));

    session->mapClass<stacuist::engine::Recipe>("recipe");

    return std::make_unique<stacuist::web::StaCuIstApplication>(
        env, std::move(session));
  });
}

