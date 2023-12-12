


// conecting to the database
db = connect('mongodb://localhost:27017/RestaurantsDB'); // Replace with your actual database URL


// task 1 
// Import restaurants.json: the collection comprises of 3772 documents.
// imported from the NOSQL manager using GUI 

// NOTE 

// make user that i imported the data with the name of the file provieded "retaurants" not "restaurants" . 
// so the collection name is "retaurants"  

// task 2
//Write a MongoDB query to display all the documents in the collection restaurants.
var allDocs = db.retaurants.find({}).limit(5);

allDocs.forEach(element => {
    printjson(element); 
});

// task 3
//Write a MongoDB query to display the fields restaurant_id, name, borough and zip code, 
//but exclude the field _id for all the documents in the collection restaurant. 


db.retaurants.find({}, { restaurant_id: 1, name: 1, borough: 1, "address.zipcode": 1, _id: 0 }).forEach(printjson);


// task 4
//Write a MongoDB query to display the first 5 restaurant which is in the borough Bronx.Cairo UniversityFaculty of EngineeringComputer Engineering Dept.CMP(N)401Lab3: NoSQL
print("------task 4 : ----------\n");
db.retaurants.find({borough : "Bronx"}).limit(5).forEach(printjson); 

// task 5 
print("---Task 5: Write a MongoDB query to display the next 5 restaurants after skipping first 5 which are in the borough Bronx---\n")
db.retaurants.find({borough : "Bronx"}).skip(5).limit(5).forEach(printjson); 


// task 6
print("------Task 6: Restaurants that do not prepare 'American' cuisine, grade > 70, and latitude < -65.754168---\n");
db.retaurants.find({
  cuisine: { $ne: "American " },
  "grades.score": { $gt: 70 },
  "address.coord.0": { $lt: -65.754168 }
}).forEach(printjson);

// task 7 
print("----Task 7: find the restaurants which belong to the borough Bronx and prepared either American or Chinese dish" )
const records = db.getCollection("retaurants").find({
  borough: "Bronx",
  $or: [{ cuisine: "American" },{ cuisine: "American " }, { cuisine: "Chinese" }]// American 
}); 
records.forEach(printjson);

// task 8
db.getCollection("retaurants").find({
   "grades.score": { $not: { $gt: 10 } } },{ restaurant_id: 1, name: 1, borough: 1, cuisine: 1 }).forEach(printjson);


print("\nTask 9: Restaurants with a score divisible by 7");
db.retaurants.find({ "grades.score": { $mod: [7, 0] } }, { restaurant_id: 1, name: 1, grades: 1 }).forEach(printjson);

print("\nTask 10: Restaurants with coord array containing a value between 42 and 52");
db.retaurants.find({ "address.coord.1": { $gt: 42, $lte: 52 } }, { restaurant_id: 1, name: 1, address: 1 }).forEach(printjson);


