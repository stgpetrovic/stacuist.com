use serde::Serialize;

#[derive(Debug, Serialize)]
pub struct Recipe {
    pub name: String,
    pub ingredients: Vec<String>,
    pub text: String,
}

pub fn build_recipe(contents: String) -> Recipe {
    let mut name = String::from("");
    let mut text = vec![];
    let mut tags = vec![];
    let mut ingredients = vec![];
    let mut phase = 0;
    for line in contents.split("\n") {
        if line.starts_with("Name:") {
            phase = 1;
            continue;
        } else if line.starts_with("Ingredients:") {
            phase = 2;
            continue;
        } else if line.starts_with("Recipe:") {
            phase = 3;
            continue;
        } else if line.starts_with("Tags:") {
            phase = 4;
            continue;
        }
        if !line.is_empty() && phase == 1 {
            name = line.to_string();
        }
        if !line.is_empty() && phase == 2 {
            ingredients.push(line.to_string());
        }
        if !line.is_empty() && phase == 3 {
            text.push(line.to_string());
        }
        if !line.is_empty() && phase == 4 {
            tags.push(line.to_string());
        }
    }

    return Recipe {
        name: name.to_string(),
        ingredients: ingredients,
        text: text.join("\n"),
    };
}
