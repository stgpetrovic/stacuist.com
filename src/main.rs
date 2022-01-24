#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use]
extern crate rocket;

use indexmap::IndexMap;
use log::info;
use rand;
use rand::Rng;
use rocket::response::Redirect;
use rocket::State;
use rocket_contrib::serve::StaticFiles;
use rocket_contrib::templates::Template;
use std::fs;

mod recipe;

struct Config {
    recipes: IndexMap<String, recipe::Recipe>,
}

#[get("/recipe/<name>")]
fn recipe_name(name: String, config: State<Config>) -> Template {
    let context = config.recipes.get(name.as_str());
    return Template::render("recipe", &context);
}

#[get("/random")]
fn random(config: State<Config>) -> Template {
    let mut rng = rand::thread_rng();
    let context = match config.recipes.get_index(
        (rng.gen::<usize>() % config.recipes.len())
            .try_into()
            .unwrap(),
    ) {
        Some(x) => Some(x.1),
        None => panic!("bug in random lookup"),
    };
    return Template::render("recipe", &context);
}

#[get("/")]
fn default() -> Redirect {
    return Redirect::to(uri!(random));
}

fn config() -> Config {
    let paths = fs::read_dir("recipes").unwrap();
    let mut r = IndexMap::new();

    for path in paths {
        let p = path.unwrap().path();
        let recipe = recipe::build_recipe(
            fs::read_to_string(p).expect("Something went wrong reading the file"),
        );
        info!("Parsing recipe: {}", recipe.name.to_string().to_lowercase());
        r.insert(recipe.name.to_string().to_lowercase(), recipe);
    }
    return Config { recipes: r };
}

fn main() {
    rocket::ignite()
        .manage(config())
        .mount("/static", StaticFiles::from("static"))
        .mount("/", routes![recipe_name, random, default])
        .attach(Template::fairing())
        .launch();
}
