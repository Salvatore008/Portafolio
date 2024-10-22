create or replace function new_compañia_celular(
				compañia varchar(30),
				costo_minuto real,
				fecha date
) 
returns void as
$$
begin
	insert into compañia_celular
	values (compañia,
			costo_minuto,
			fecha
			 );
end;
$$
language plpgsql;

select new_compañia_celular('MOV',
							14.6,
							'2020/07/01'
							);

select new_compañia_celular('TEL',
							15.0,
							'2020/09/01'
							);




