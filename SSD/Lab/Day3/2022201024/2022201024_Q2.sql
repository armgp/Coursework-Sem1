select concat(emp.Fname,' ',emp.Minit,' ',emp.Lname), a.Mgr_ssn, emp.Dno, a.noOfEmployees
from EMPLOYEE emp,
(select d.Mgr_ssn, count(e.Ssn) as noOfEmployees
from DEPARTMENT d, EMPLOYEE e
where d.Mgr_ssn=e.Super_ssn
group by d.Mgr_ssn) as a
where a.Mgr_ssn=emp.Ssn
order by Mgr_ssn asc
