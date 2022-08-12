const express = require('express');
const Joi = require('joi'); //used for validation
const app = express();
app.use(express.json());
 
const publicips = [
{ip: '192.168.1.1', cmd: 'unkonwn', date:'unknown'}
]
 
//READ Request Handlers
app.get('/', (req, res) => {
res.send('Welcome you!');
});
 
app.get('/api/ips', (req,res)=> {
res.send(publicips);
});
 
//app.get('/api/books/:id', (req, res) => {
//const book = books.find(c => c.id === parseInt(req.params.id));
 
//if (!book) res.status(404).send('<h2 style="font-family: Malgun Gothic; color: darkred;">Ooops... Cant find what you are looking for!</h2>');
//res.send(book);
//});
 
//CREATE Request Handler
app.post('/api/ips', (req, res)=> {
 
//const { error } = validateIpinfo(req.body);
//if (error){
//res.status(400).send(error.details[0].message)
//return;
//}
const myip = {
ip: req.body.ip,
cmd: req.body.cmd,
date: req.body.date
};
publicips.pop();
publicips.push(myip);
res.send(myip);
});
 
//UPDATE Request Handler
//app.put('/api/books/:id', (req, res) => {
//const book = books.find(c=> c.id === parseInt(req.params.id));
//if (!book) res.status(404).send('<h2 style="font-family: Malgun Gothic; color: darkred;">Not Found!! </h2>');
 
//const { error } = validateBook(req.body);
//if (error){
//res.status(400).send(error.details[0].message);
//return;
//}
 
//book.title = req.body.title;
//res.send(book);
//});
 
//DELETE Request Handler
//app.delete('/api/books/:id', (req, res) => {
 
//const book = books.find( c=> c.id === parseInt(req.params.id));
//if(!book) res.status(404).send('<h2 style="font-family: Malgun Gothic; color: darkred;"> Not Found!! </h2>');
 
//const index = books.indexOf(book);
//books.splice(index,1);
 
//res.send(book);
//});
 
function validateIpinfo(ipinfo) {
const schema = Joi.object().keys({
ip: Joi.string().min(7).required(),
date: Joi.string().required(),
cmd: Joi.string().required() 
});
return Joi.validate(ipinfo, schema);
 
}
 
//PORT ENVIRONMENT VARIABLE
//const port = process.env.PORT || 8080;
const port = 20038;
app.listen(port, () => console.log(`Listening on port ${port}..`));

