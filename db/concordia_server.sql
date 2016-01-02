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
    target_segment text,
    source_tokens integer[],
    target_tokens integer[]
);

DROP TABLE IF EXISTS alignment;
CREATE TABLE alignment (
    id SERIAL PRIMARY KEY,
    unit_id integer,
    source_token_pos integer,
    target_token_pos integer
);

CREATE INDEX ON alignment(unit_id, source_token_pos);
