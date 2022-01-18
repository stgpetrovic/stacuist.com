use crate::recipe::Recipe;
use std::option::Option;

pub fn make_template(recipe: Option<&Recipe>) -> String {
    return format!("Riceta: {:?}", recipe);
}
