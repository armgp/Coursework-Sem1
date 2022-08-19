select b.Dept_Id, b.Dept_name, count(dl.Dlocation) as numberOfLocations
from DEPT_LOCATIONS dl,
(select Dnumber as Dept_Id, Dname as Dept_name
from DEPARTMENT dp,
(select Mgr_ssn, count(Sex) as fdep
from DEPARTMENT, DEPENDENT
where Mgr_ssn=Essn and Sex='F' 
group by Mgr_ssn) a 
where a.Mgr_ssn=dp.Mgr_ssn and a.fdep>=2) as b
where b.Dept_Id=dl.Dnumber
group by b.Dept_Id

