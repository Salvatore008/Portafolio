create or replace function new_sucursal(
				codigo int,
				nombre varchar(30),
				abreviatura varchar(30),
				tipo_destino varchar(30),
				fecha date default null,
				costo real default null
) 
returns void as
$$
begin
	
	insert into sucursal 
	values (codigo,
	        nombre,
			abreviatura,
			costo,
			tipo_destino,
			fecha
			 );
end;
$$
language plpgsql;

select new_sucursal(0212,
					'Caracas',
					'CAR',
					'nacional'
					);

select new_sucursal(0232,
					'Los Teques',
					'LTQ',
					'nacional'
					);

select new_sucursal(0233,
					'Carayaca',
					'CAZ',
					'nacional'
					);

select new_sucursal(0234,
					'Los Velásquez',
					'AVE',
					'nacional'
					);

select new_sucursal(034,
					'España',
					'ESP',
					'internacional',
					'2020/08/01',
					9.80
					);

select new_sucursal(039,
					'Italia',
					'ITA',
					'internacional',
					'2020/08/01',
					10.00
					);

select new_sucursal(001,
					'Estados Unidos de América',
					'EUA',
					'internacional',
					'2020/08/01',
					7.60
					);

select new_sucursal(000,
					'Movilcel',
					'MOV',
					'nacional',
					'2020/07/01',
					14.60
					);

select new_sucursal(000,
					'Telenet',
					'TEL',
					'nacional',
					'2020/09/01',
					15.00
					);









