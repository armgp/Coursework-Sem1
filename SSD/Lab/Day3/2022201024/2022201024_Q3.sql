select a.Mgr_ssn, count(b.Pno) as number_of_projects
from 
(select Mgr_ssn
from DEPARTMENT d, PROJECT p
where p.Pname='ProductY' and p.Dnum=d.Dnumber) a,
WORKS_ON b
where  b.Essn=a.Mgr_ssn
group by a.Mgr_ssn