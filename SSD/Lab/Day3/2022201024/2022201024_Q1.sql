select concat(e1.Fname, ' ', e1.Minit, ' ', e1.Lname) as FullName, e1.Ssn, e1.Dept_Id, d1.Dname as Dept_Name
from DEPARTMENT d1,
(select Fname, Minit, Lname, Ssn, Dno as Dept_Id from EMPLOYEE e,
(select Super_ssn
from EMPLOYEE e,
(select Essn, sum(Hours)
from WORKS_ON
group by Essn
having sum(Hours)<40) as a
where e.Ssn = a.Essn) as b
where e.Ssn=b.Super_ssn) as e1
where d1.Dnumber=e1.Dept_Id