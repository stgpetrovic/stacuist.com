create table recipe (
  id integer primary key autoincrement,
  version integer not null,
  name text not null,
  author text not null,
  text text not null,
  ingredients text not null
);

create table tag (
  id integer primary key autoincrement,
  name text not null
);

create table recipe_tag (
  recipe_id integer not null,
  tag_id integer not null,
  primary key (recipe_id, tag_id),
  constraint fk_recipe foreign key (recipe_id) references recipe (id) on delete cascade deferrable initially deferred,
  constraint fk_tag foreign key (tag_id) references tag (id) on delete cascade deferrable initially deferred
);

create index recipe_tag_recipe on recipe_tag (recipe_id);
create index recipe_tag_tag on recipe_tag (tag_id);
