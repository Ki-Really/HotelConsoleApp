# HotelConsoleApp
Project for admins of hotel. Made using C++, PostgreSQL, ODBC. There is an opportunity to add person, maid, schedule etc. to database. Opportunity to update and delete entities. Created console interface 
for making admins work easier. 
Here I used DataMapper pattern: 
- Every entity has its mapper class.
- Mapper is used to separate work with database and entity.

## Console app looks like
![image](https://github.com/Ki-Really/HotelConsoleApp/assets/133647432/a392238e-e260-4c8c-9d15-932979527c4e)

## Guest entity
![image](https://github.com/Ki-Really/HotelConsoleApp/assets/133647432/8deff822-5738-410a-b454-8b9693765b34)

## Guest adding 
![image](https://github.com/Ki-Really/HotelConsoleApp/assets/133647432/f7c40ef3-4cec-43e6-8cab-efffb20bd1ee)

## Rooms list
![image](https://github.com/Ki-Really/HotelConsoleApp/assets/133647432/4d076472-763a-4ffc-b5a0-ca7cbd3cc3eb)

## Database scheme looks like
![image](https://github.com/Ki-Really/HotelConsoleApp/assets/133647432/8bac5099-0294-4726-b562-974d4ce1c9ca)


- Table guest– guest data (passport, address, etc.). 
- Table address – address data.
- Table passport – passport data.
- Table room – room data.
- Table maid – maid data (address, name, surname, patronymic).
- Table schedule – schedule of cleaning (for each maid).
- Table service – additional service for guests (spa, swimming pool, PS4 etc.).
