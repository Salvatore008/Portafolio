create or replace function new_cuesta(
				origen varchar(30),
				codigo_origen int,
				destino varchar(30),
				codigo_destino int,
				costo real,
				fecha date
) 
returns void as
$$
begin
	insert into cuesta
	values (origen,
			codigo_origen,
			destino,
			codigo_destino,
			costo,
			fecha
			 );
end;
$$
language plpgsql;

select new_cuesta('AVE',
					0234,
					'CAR',
					0212,
					5.80,
					'2020/08/01'
					);

select new_cuesta('AVE',
					0234,
					'LTQ',
					0232,
					3.00,
					'2020/08/01'
					);

select new_cuesta('AVE',
					0234,
					'CAZ',
					0233,
					2.70,
					'2020/08/01'
					);

select new_cuesta('CAR',
					0212,
					'LTQ',
					0232,
					4.50,
					'2020/08/01'
					);

select new_cuesta('CAR',
					0212,
					'CAZ',
					0233,
					3.60,
					'2020/08/01'
					);

select new_cuesta('CAZ',
					0233,
					'LTQ',
					0232,
					2.50,
					'2020/08/01'
					);


select * from cuesta;










