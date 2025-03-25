const rest = require('../modele/rest');

// GET /api/identifiants
exports.getIdentifiants = async (req, res) => {
  try {
    const identifiants = await rest.getIdentifiants();
    res.json(identifiants);
  } catch (err) {
    console.log(" ERREUR SERVEUR :", err); // Affiche l'erreur complète
    res.status(500).json({ error: 'Erreur lors de la récupération des identifiants.' });
  }
};

exports.getSignificationsTest = async (req, res) => {
  try {
    const significationstest = await rest.getSignificationsTest();
    res.json(significationstest);
  } catch (err) {
    console.log(" ERREUR SERVEUR :", err); // Affiche l'erreur complète
    res.status(500).json({ error: 'Erreur lors de la récupération des significations test.' });
  }
};

exports.getSignifications = async (req, res) => {
  const id = req.params.id;
  // Vérifier que l'ID contient uniquement lettres et chiffres
  if (!/^[a-zA-Z0-9]+$/.test(id)) {
    return res.status(400).json({ error: "ID invalide." });
  }
  try {
    const significations = await rest.getSignifications(id);
    res.json(significations);
  } catch (error) {
    res.status(500).json({ error: "Erreur serveur" });
  }
};

exports.getSignificationsUnOctet = async (req, res) => {
  const id = req.params.id;
  const octet = req.params.octet;
  // Vérification des paramètres (chiffres et lettres uniquement)
  if (!/^[a-zA-Z0-9]+$/.test(id) || !/^\d+$/.test(octet)) {
    return res.status(400).json({ error: "ID ou Octet invalide" });
  }
  try {
    const significations = await rest.getSignificationsUnOctet(id, octet);
    res.json(significations);
  } catch (error) {
    res.status(500).json({ error: "Erreur serveur" });
  }
};

exports.PostTrame = async (req, res) => {
  try {
    console.log("Requête reçue :", req.body);

    const { users, data } = req.body; // Extraction des données du JSON

    // Vérifier si le JSON est valide
    if (!users || !Array.isArray(data)) {
      return res.status(400).json({ error: "Format JSON invalide" });
    }

    // Appel de la fonction `PostTrame` depuis `rest.js`
    const response = await rest.PostTrame(req.body);

    res.json({ message: "Trames CAN traitées avec succès !", details: response });

  } catch (error) {
    console.error("Erreur dans apiController :", error);
    res.status(500).json({ error: "Erreur serveur" });
  }
};

exports.getTramesParId = async (req, res) => {
  const id = req.params.id;
  // Vérifier que l'ID contient uniquement lettres et chiffres
  if (!/^[a-zA-Z0-9]+$/.test(id)) {
    return res.status(400).json({ error: "ID invalide." });
  }
  try {
    const TramesParId = await rest.getTramesParId(id);
    res.json(TramesParId);
  } catch (error) {
    res.status(500).json({ error: "Erreur serveur" });
  }
};

exports.lien = async (req,res) => {
  lien = "https://prod.liveshare.vsengsaas.visualstudio.com/join?02CB741BAE65E711C7B1D120573B63E28B70"
  res.send(lien)
}
// exports.PostTrame = async (req, res) => {
//   try {
//     //console.log(req.body.users);
//     const { users, data } = req.body; // Extraction des données du JSON
    
//     // Vérifier si le JSON est valide
//     if (!users || !Array.isArray(data)) {
//       return res.status(400).json({ error: "Format JSON invalide" });
//     }
//     //console.log('pipi');
//     // Appel de la fonction dans `rest.js`
//    // console.log(req.body);
//     const response = await rest.PostTrame(req.body);
//     //console.log("caca");
//     res.json(response);

//   } catch (error) {
//     res.status(500).json({ error: "Erreur serveur" });
//   }
// };
