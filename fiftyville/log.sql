-- Get crime_scene report id and description
SELECT id, description
FROM crime_scene_reports
WHERE year = 2024
AND month = 7
AND day = 28
AND street = 'Humphrey Street';

-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time –
-- each of their interview transcripts mentions the bakery.

-- narrowed down id is 295 additionly mentions 3 interviews on same day all mentioning a bakery
SELECT id, name, transcript
FROM interviews
WHERE year = 2024
AND month = 7
AND day = 28
AND transcript LIKE '%bakery%';

-- Interview 161: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking
-- lot and drive away. If you have security footage from the bakery parking lot, you might want to
-- look for cars that left the parking lot in that time frame.

-- get info on bakery security logs
SELECT id, activity, license_plate
FROM bakery_security_logs
WHERE year = 2024
AND month = 7
AND day = 28
AND hour = 10
AND minute BETWEEN 15 AND 25
and activity = 'exit';


-- Interview 162: I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I
-- arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing
-- some money.

-- get info on atm transactions
SELECT id, account_number, amount
FROM atm_transactions
WHERE year = 2024
AND month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';

-- Interview 163: As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- get info on phone calls
SELECT id, caller, receiver
FROM phone_calls
WHERE year = 2024
AND month = 7
AND day = 28
AND duration < 60;

-- get tommorow earliest flight
SELECT id, destination_airport_id, month, day, hour, minute
FROM flights
WHERE year = 2024
AND month = 7
AND day = 29
ORDER BY hour, minute;

-- get passenger info on flight
SELECT passport_number, seat
FROM passengers
WHERE flight_id =
    (SELECT id
    FROM flights
    WHERE year = 2024
    AND month = 7
    AND day = 29
    ORDER BY hour, minute);




-- get theif name--------------------------
SELECT name
FROM people
WHERE id IN
    (SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
        (SELECT account_number
        FROM atm_transactions
        WHERE year = 2024
        AND month = 7
        AND day = 28
        AND atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'))
    AND phone_number IN
        (SELECT caller
        FROM phone_calls
        WHERE year = 2024
        AND month = 7
        AND day = 28
        AND duration < 60)
    AND passport_number IN
        (SELECT passport_number
        FROM passengers
        WHERE flight_id =
            (SELECT id
            FROM flights
            WHERE year = 2024
            AND month = 7
            AND day = 29
            ORDER BY hour, minute))
    AND license_plate IN
        (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2024
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute BETWEEN 15 AND 25
        and activity = 'exit');

-- get location theif escaped to ------------
SELECT city
FROM airports
WHERE id =
    (SELECT destination_airport_id
    FROM flights
    WHERE year = 2024
    AND month = 7
    AND day = 29
    ORDER BY hour, minute
    LIMIT 1);

-- get accomplice
    SELECT name
    FROM people
    WHERE phone_number IN
    (SELECT receiver
    FROM phone_calls
    WHERE year = 2024
    AND month = 7
    AND day = 28
    AND duration < 60
    AND caller =
        (SELECT phone_number
        FROM people
        WHERE name = 'Bruce'));


