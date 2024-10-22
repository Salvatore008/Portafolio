--Eliminar la base de datos

-- Se verifica que no haya ninguna actividad
SELECT * FROM pg_stat_activity WHERE datname='proyecto';

--Si hay procesos activos, termina su acceso a la base de datos
SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE datname='proyecto';

-- Una vez que no haya procesos activos, puedes eliminar la base de datos
drop database proyecto;