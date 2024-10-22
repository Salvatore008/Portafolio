create or replace function new_servicio(
				abreviatura varchar(30),
				descripcion varchar(30),
				costo real
) 
returns void as
$$
begin
	insert into servicio
	values (abreviatura,
			descripcion,
			costo
			 );
end;
$$
language plpgsql;

select new_servicio('STA',
					'Servicio de Teleamigo',
					200.0
					);

select new_servicio('SNP',
					'Servicio de Número Privado',
					400.0
					);

select * from servicio;


