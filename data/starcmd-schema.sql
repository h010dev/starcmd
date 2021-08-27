-- tables
-- Table: commands
CREATE TABLE commands (
  id          integer       primary key autoincrement,
  name        varchar(255)  not null,
  platform    varchar(255)  not null,
  os          varchar(255)  null,
  description varchar(4096) null,
  command     varchar(4096) not null,
  examples    varchar(4096) null,
  refs        varchar(4096) null,
  tags        varchar(255)  null,
  datemod     varchar(256)  not null,
  fav         int           default 0
);
