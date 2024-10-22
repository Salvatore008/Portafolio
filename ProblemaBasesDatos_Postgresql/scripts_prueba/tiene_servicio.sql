create or replace function new_tiene_servicio(
				id_servicio varchar(30),
				id_contrato varchar(30),
				fecha_inicio date,
				fecha_final date
) 
returns void as
$$
begin
	insert into tiene_servicio
	values (id_servicio,
			id_contrato,
			fecha_inicio,
			fecha_final
			 );
end
$$
language plpgsql;

select new_tiene_servicio('STA',
						'0212-6622590',
						'2020/10/01',
						'2020/10/25'
						);

select new_tiene_servicio('SNP',
						'0212-6622590',
						'2020/10/01',
						'2020/10/25'
						);

select new_tiene_servicio('STA',
						'0212-4844356',
						'2020/10/19',
						'2020/11/13'
						);

select * from tiene_servicio;
