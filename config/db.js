const { Pool } = require('pg');

// Configuration de la connexion
const db = new Pool({
  host: hostBDD,     // Adresse du serveur MySQL
  user: process.env.userBDD,          // Nom d'utilisateur
  password: process.env.mdpBDD,  // Mot de passe
  database: process.env.nomBDD,   // Nom de la base de données
  port: process.env.portBDD,
});

db.connect()
  .then(() => console.log(" Connecté à PostgreSQL !"))
  .catch(err => console.error(" Erreur de connexion PostgreSQL :", err));

module.exports = db;