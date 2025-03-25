require('dotenv').config();
const express = require('express');
const app = express();
const path = require('path');
const cors = require('cors');

// Middleware
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.use(express.static(path.join(__dirname, 'public')));
app.use(cors());

// Routes
const indexRoutes = require('./routes/web/index');
const apiRoutes = require('./routes/api/api');

// Routes
app.use('/api', apiRoutes);
app.use('/', indexRoutes);
app.set('view engine','ejs');
app.set('views ','./views');

// Démarrer le serveur
const PORT = 3000;
app.listen(PORT, () => {
  console.log(` Serveur démarré sur http://localhost:${PORT}`);
});