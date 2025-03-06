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
    console.log("📩 Requête reçue :", req.body);

    const { users, data } = req.body; // Extraction des données du JSON

    // Vérifier si le JSON est valide
    if (!users || !Array.isArray(data)) {
      return res.status(400).json({ error: "Format JSON invalide" });
    }

    // Appel de la fonction `PostTrame` depuis `rest.js`
    const response = await rest.PostTrame(req.body);

    res.json({ message: "Trames CAN traitées avec succès !", details: response });

  } catch (error) {
    console.error("❌ Erreur dans apiController :", error);
    res.status(500).json({ error: "Erreur serveur" });
  }
};
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



// GET /api/items/:id
// exports.getItemById = async (req, res) => {
//   try {
//     const id = parseInt(req.params.id, 10);
//     const item = await rest.getById(id);
//     if (item) {
//       res.json(item);
//     } else {
//       res.status(404).json({ error: 'Item introuvable.' });
//     }
//   } catch (err) {
//     res.status(500).json({ error: 'Erreur lors de la récupération de l’item.' });
//   }
// };

// // POST /api/items
// exports.createItem = async (req, res) => {
//   try {
//     const newItem = req.body;
//     const createdItem = await rest.add(newItem);
//     res.status(201).json(createdItem);
//   } catch (err) {
//     res.status(500).json({ error: 'Erreur lors de la création de l’item.' });
//   }
// };

// // PUT /api/items/:id
// exports.updateItem = async (req, res) => {
//   try {
//     const id = parseInt(req.params.id, 10);
//     const updatedItem = await rest.update(id, req.body);
//     if (updatedItem) {
//       res.json(updatedItem);
//     } else {
//       res.status(404).json({ error: 'Item introuvable.' });
//     }
//   } catch (err) {
//     res.status(500).json({ error: 'Erreur lors de la mise à jour de l’item.' });
//   }
// };

// // DELETE /api/items/:id
// exports.deleteItem = async (req, res) => {
//   try {
//     const id = parseInt(req.params.id, 10);
//     const success = await rest.delete(id);
//     if (success) {
//       res.json({ message: 'Item supprimé avec succès.' });
//     } else {
//       res.status(404).json({ error: 'Item introuvable.' });
//     }
//   } catch (err) {
//     res.status(500).json({ error: 'Erreur lors de la suppression de l’item.' });
//   }
// };
