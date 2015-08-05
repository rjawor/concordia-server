DROP TABLE IF EXISTS tm;
CREATE TABLE tm (
    id SERIAL PRIMARY KEY,
    source_lang_id integer,
    target_lang_id integer,
    name varchar(40)
);

DROP TABLE IF EXISTS language;
CREATE TABLE language (
    id SERIAL PRIMARY KEY,
    code varchar(10),
    name varchar(30)
);

DROP TABLE IF EXISTS unit;
CREATE TABLE unit (
    id SERIAL PRIMARY KEY,
    tm_id integer,
    source_segment text,
    target_segment text
);
