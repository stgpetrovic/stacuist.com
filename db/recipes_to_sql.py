#!/bin/env python3

import os
from collections import namedtuple

Recipe = namedtuple('Recipe', ['name', 'ingredients', 'text', 'tags'])

def ParseRecipes():
    recipes = []
    for fn in os.listdir("./recipes"):
        ingredients = []
        text = []
        tags = []
        name = ""
        phase = None
        for l in open("./recipes/"+fn).read().splitlines():
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
    for recipe in recipes:
        for tag in recipe.tags:
            if tag not in tags:
                tags[tag] = len(tags)

    for tag, key in tags.items():
        sql.append('insert into tag values(%d, 0, "%s")' % (key, tag))

    for key, recipe in enumerate(recipes):
        sql.append('insert into recipe values(%d, 0, "%s", "slon", "%s", "%s")' % (key, recipe.name, '\n'.join(recipe.text), '|'.join(recipe.ingredients)))
        for tag in recipe.tags:
            sql.append('insert into recipe_tags values(%s, %d)' % (key, tags[tag]))

    return ';\n'.join(sql)


if __name__ == '__main__':
    print(open('db.sql').read())
    print(ToSql(ParseRecipes()))
