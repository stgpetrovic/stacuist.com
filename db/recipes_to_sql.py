#!/bin/env python3

import os
from collections import namedtuple

from rules_python.python.runfiles import runfiles

Recipe = namedtuple('Recipe', ['name', 'ingredients', 'text', 'tags'])

def ParseRecipes():
    recipes = []
    r = runfiles.Create()

    root = r.Rlocation("stacuist/db/recipes")
    for fn in os.listdir(root):
        ingredients = []
        text = []
        tags = []
        name = ""
        phase = None
        for l in open(os.path.join(root,fn)).read().splitlines():
            if l == "Name:":
                phase = 0
                continue
            if l == "Ingredients:":
                phase = 1
                continue
            elif l == "Recipe:":
                phase = 2
                continue
            elif l == "Tags:":
                phase = 3
                continue
            if l and phase == 0:
                name = l
            if l and phase == 1:
                ingredients.append(l)
            if phase == 2:
                text.append(l)
            if l and phase == 3:
                tags.append(l)
        if text and not text[-1]:
            text = text[:-1]
        recipes.append(Recipe(name,ingredients,text,tags))
    return recipes

def ToSql(recipes):
    sql = []

    tags = {}
    for recipe in sorted(recipes):
        for tag in sorted(recipe.tags):
            if tag not in tags:
                tags[tag] = len(tags)

    for tag, key in tags.items():
        sql.append('insert into tag values(%d, 0, "%s")' % (key, tag))

    for key, recipe in enumerate(sorted(recipes)):
        sql.append('insert into recipe values(%d, 0, "%s", "slon", "%s", "%s")' % (key, recipe.name, '\n'.join(recipe.text), '|'.join(recipe.ingredients)))
        for tag in recipe.tags:
            sql.append('insert into recipe_tags values(%s, %d)' % (key, tags[tag]))

    return ';\n'.join(sql)


if __name__ == '__main__':
    r = runfiles.Create()
    with open(r.Rlocation("stacuist/db/db.sql"), "r") as f:
      print(f.read())
      print(ToSql(ParseRecipes()))
