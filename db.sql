create table recipe (
  id integer primary key autoincrement,
  version integer not null,
  name text not null,
  author text not null,
  text text not null,
  tags text
);

create table ingredient (
  id integer primary key autoincrement,
  name text not null,
  quantity float not null
);

create table recipe_ingredient (
  recipe_id integer not null,
  ingredient_id integer not null,
  primary key (recipe_id, ingredient_id),
  constraint fk_recipe foreign key (recipe_id) references recipe (id) on delete cascade deferrable initially deferred,
  constraint fk_ingredient foreign key (ingredient_id) references ingredient (id) on delete cascade deferrable initially deferred
);

create index recipe_ingredient_recipe on recipe_ingredient (recipe_id);
create index recipe_ingredient_ingredient on recipe_ingredient (ingredient_id);
