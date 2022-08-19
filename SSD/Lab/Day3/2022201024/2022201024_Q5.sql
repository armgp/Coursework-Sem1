select t.Mgr_ssn, dpt.Dnumber, t.noOfDependents
from DEPARTMENT dpt,
(select d.Mgr_ssn, count(dt.Dependent_name) as noOfDependents
from DEPENDENT as dt,
(select b.Mgr_ssn, d.Dnumber
from DEPARTMENT d,
(select a.Mgr_ssn
from
(select d.Mgr_ssn, count(dl.Dlocation) as locs
from DEPARTMENT d, DEPT_LOCATIONS dl
where d.Dnumber=dl.Dnumber 
group by Mgr_ssn) as a
where a.locs>=2) as b
where d.Mgr_ssn=b.Mgr_ssn) as d
where dt.Essn=d.Mgr_ssn
group by Mgr_ssn) as t
where dpt.Mgr_ssn=t.Mgr_ssn