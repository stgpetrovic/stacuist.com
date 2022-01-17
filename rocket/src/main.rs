#[macro_use]
extern crate rocket;

use rocket::State;
use std::collections::HashMap;
use std::fs;

mod recipe;

struct Config {
    recipes: HashMap<String, recipe::Recipe>,
}

#[get("/recipe/<name>")]
fn index(name: &str, config: &State<Config>) -> String {
    if !config.recipes.contains_key(name) {
        return format!("Nima ricete: {}!", name);
    }
    return format!("Riceta: {:?}", config.recipes.get(name));
}

fn config() -> Config {
    let paths = fs::read_dir("../db/recipes").unwrap();
    let mut r = HashMap::new();

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

#[rocket::main]
async fn main() -> Result<(), rocket::Error> {
    rocket::build()
        .manage(config())
        .mount("/", routes![index])
        .ignite()
        .await?
        .launch()
        .await
}
