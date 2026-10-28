LOAD data/students.csv
STATS
INSERT 100500 "Aylin Demir" "Felsefe" 3.72 2
SELECT id = 100500
SELECT gpa BETWEEN 3.0 3.5
SELECT dept = "Bilgisayar"
SELECT dept = "Felsefe"
DELETE 100500
SELECT id = 100500
STATS
EXIT
