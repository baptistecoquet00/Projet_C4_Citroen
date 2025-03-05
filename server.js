require('dotenv').config();
const express = require('express');
const app = express();
const path = require('path');
const cors = require('cors');

app.use(cors());

// Middleware
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));

// Routes
const indexRoutes = require('./routes/index');
const apiRoutes = require('./routes/api');


// Routes
app.use('/', indexRoutes);
app.use('/api', apiRoutes);

// Démarrer le serveur
const PORT = 3000;
app.listen(PORT, () => {
  console.log(` Serveur démarré sur http://localhost:${PORT}`);
});