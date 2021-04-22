#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <glog/logging.h>

#include "absl/algorithm/container.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/types/span.h"
#include "engine/recipe.h"
#include "web/tags.h"

namespace stacuist::web {

using RecipeInfo = std::tuple<std::string, std::string, std::string>;

class StaCuIstApplication : public Wt::WApplication {
 public:
  StaCuIstApplication(const Wt::WEnvironment &env,
                      std::unique_ptr<Wt::Dbo::Session> session);

  std::string GetRecipe(absl::Span<const std::string> tags);

 private:
  Wt::WText *recipe_text_;
  std::unique_ptr<Wt::Dbo::Session> session_;
};

std::string StaCuIstApplication::GetRecipe(absl::Span<const std::string> tags) {
  VLOG(1) << "Finding recipes for " << absl::StrJoin(tags, ", ");
  Wt::Dbo::Transaction transaction{*session_};
  std::string matcher = "1=1";
  if (!tags.empty()) {
    matcher = "";
    for (const auto &tag : tags) {
      if (!matcher.empty()) {
        matcher += " and ";
      }
      matcher += absl::StrCat("t.name like '", tag, "'");
    }
  }
  Wt::Dbo::collection<RecipeInfo> recipe =
      session_
          ->query<RecipeInfo>(absl::StrCat(
              "select r.name, r.text, r.ingredients from recipe r where id in "
              "( "
              "select r.id from "
              "recipe r join recipe_tags rt on "
              "r.id = rt.recipe_id join tag t on t.id = rt.tag_id "
              "where ",
              matcher, ")"))
          .limit(1)
          .orderBy("random()")
          .resultList();
  if (recipe.size() > 0) {
    auto it = *recipe.begin();
    return absl::StrCat(std::get<0>(it), ":", std::get<1>(it));
  }
  return "< no recipe found >";
}

StaCuIstApplication::StaCuIstApplication(
    const Wt::WEnvironment &env, std::unique_ptr<Wt::Dbo::Session> session)
    : Wt::WApplication(env), session_(std::move(session)) {
  setTitle("Šta ću ist?!");

  {
    Wt::Dbo::Transaction transaction{*session_};
    Wt::Dbo::collection<Wt::Dbo::ptr<engine::Tag>> tags =
        session_->find<engine::Tag>();
    root()->addWidget(std::make_unique<TagsWidget>(
        tags, [this](absl::Span<const std::string> tags) {
          recipe_text_->setText(GetRecipe(tags));
        }));
  }

  root()->addWidget(std::make_unique<Wt::WBreak>());

  recipe_text_ = root()->addWidget(std::make_unique<Wt::WText>());
  recipe_text_->setText(GetRecipe({"vegan"}));
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

