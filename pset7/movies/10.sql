SELECT DISTINCT(people.name) FROM people
JOIN directors ON people.id = directors.person_id
JOIN movies ON directors.movie_id = movies.id
WHERE movies.id IN
(SELECT movie_id FROM ratings WHERE rating >= 9.0);