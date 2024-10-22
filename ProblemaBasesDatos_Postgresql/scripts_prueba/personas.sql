create or replace function new_persona_natural(
				cedula varchar(30),
				nombre varchar(30),
				apellido varchar(30)
) 
returns void as
$$
declare
	id_suscriptor int;
begin
	insert into suscriptor values (default);
	
	select id into id_suscriptor from suscriptor
	order by id desc limit 1;
	
	insert into persona_natural 
	values ( id_suscriptor,
	         cedula,
			 nombre,
			 apellido
			 );
end;
$$
language plpgsql;

create or replace function new_persona_juridica(
				rif varchar(30),
				razon_social varchar(30)
) 
returns void as
$$
declare
	id_suscriptor int;
begin
	insert into suscriptor values (default);
	
	select id into id_suscriptor from suscriptor
	order by id desc limit 1;
	
	insert into persona_juridica
	values (rif,
			razon_social,
			id_suscriptor
			 );
end;
$$
language plpgsql;

select new_persona_natural(
	        'V-4.890.639',
			'José Eladio',
			 'Martínez Díaz'
			 );

select new_persona_natural(
	        'V-6.345.012',
			'Milex',
			 'Deris Nieto'
			 );

select new_persona_natural(
	        'V-3.855.721',
			'Vicenta',
			 'Rivero de Bracho'
			 );

select new_persona_juridica(
	        'J-02690345-7',
			'Sistemas J.J y Asociados'
			 );











