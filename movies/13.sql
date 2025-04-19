SELECT name FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE movie_id IN(
    SELECT movie_id FROM stars
    JOIN movies ON stars.movie_id = movies.id
    JOIN people ON stars.person_id = people.id
    WHERE name = 'Kevin Bacon'
)
AND name != 'Kevin Bacon';


