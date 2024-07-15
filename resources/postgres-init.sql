
CREATE TABLE personal_data (
    id SERIAL PRIMARY KEY,
    forename VARCHAR(50),
    family_name VARCHAR(50),
    age SMALLINT
);
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    personal_data_id INTEGER REFERENCES personal_data(id),
    login VARCHAR(50),
    password VARCHAR(50),
    email VARCHAR(100)
);
