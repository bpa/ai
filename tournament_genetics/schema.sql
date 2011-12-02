create table individual (
  individual_id integer primary key autoincrement
);

create table genes (
  gene_id integer primary key autoincrement,
  individual_id int not null,
  marker int not null,
  value text
);

create table markers (
  marker text primary key,
  datatype text,
  validation text
);

insert into markers values ('distance', 'range', '1..10');
insert into markers values ('aim', 'set', 'true,false');
