const db = require('../config/db');

// Récupérer tous les items
exports.getIdentifiants = async () => {
  const result = await db.query('SELECT can_id FROM identifiants_can');
  console.log(result)
  return result.rows;
};

exports.getSignificationsTest= async () => {
  const result = await db.query('SELECT * FROM significations_can');
  console.log(result)
  return result.rows;
};

exports.getSignifications = async (id) => {
  try {
    const query = "SELECT * FROM significations_can WHERE can_id = $1 ORDER BY octet ASC";
    const result = await db.query(query, [id]); // Pas de conversion, id reste une chaîne
    console.log(result.rows);
    return result.rows;
  } catch (error) {
    console.error("Erreur lors de la récupération des significations :", error);
    throw error;
  }
};
exports.getTramesParId = async (id) => {
  try {
    const query = "SELECT * FROM trames_can WHERE can_id = $1 ORDER BY horodatage ASC";
    const result = await db.query(query, [id]); // Deux paramètres dans la requête
    console.log(result.rows);
    return result.rows;
  } catch (error) {
    console.error("Erreur lors de la récupération des trames par id  :", error);
    throw error;
  }
};
exports.getSignificationsUnOctet = async (id, octet) => {
  try {
    const query = "SELECT can_id,octet,signification FROM significations_can WHERE can_id = $1 AND octet = $2";
    const result = await db.query(query, [id, octet]); // Deux paramètres dans la requête
    console.log(result.rows);
    return result.rows;
  } catch (error) {
    console.error("Erreur lors de la récupération des significations :", error);
    throw error;
  }
};

///////////////////////////////////////////////////////////////     POST     /////////////////////////////////////////////////////////////////////////////////////////

exports.PostTrame = async (body) => {
  try {
    const { users, data } = body;

    // Vérification du format JSON
    if (!users || !Array.isArray(data)) {
      return { error: "Format JSON invalide" };
    }

    const errors = []; // Pour collecter les trames ignorées

    for (const trame of data) {
      let { idCAN, lenData, Data } = trame;

      // Nettoyage du "0x" et mise en majuscule
      idCAN = idCAN.replace(/^0x/i, '').toUpperCase();
      Data = Data.replace(/^0x/i, '').toUpperCase();

      // ✅ Seules les Data de 16 caractères (8 octets) sont acceptées
      if (Data.length !== 16) {
        errors.push(`Trame ignorée : Data doit contenir exactement 8 octets (16 caractères) pour idCAN ${idCAN}, reçu : ${Data.length} caractères.`);
        continue; // Passe à la suivante
      }

      // ✅ Découper en octets (paires de 2 caractères)
      const octets = Data.match(/.{2}/g); // Va donner un tableau de 8 éléments

      // ✅ Vérifier si le can_id existe déjà dans identifiants_can
      const checkQuery = `SELECT COUNT(*) FROM identifiants_can WHERE can_id = $1`;
      const checkResult = await db.query(checkQuery, [idCAN]);

      if (parseInt(checkResult.rows[0].count) === 0) {
        // ✅ Ajouter can_id s'il n'existe pas
        const insertIdentifiantQuery = `INSERT INTO identifiants_can (can_id) VALUES ($1)`;
        await db.query(insertIdentifiantQuery, [idCAN]);
      }

      // ✅ Insérer la trame complète
      const insertTrameQuery = `
        INSERT INTO trames_can (can_id, horodatage, octet_0, octet_1, octet_2, octet_3, octet_4, octet_5, octet_6, octet_7)
        VALUES ($1, NOW(), $2, $3, $4, $5, $6, $7, $8, $9)
      `;
      await db.query(insertTrameQuery, [idCAN, ...octets]);
    }

    // ✅ Réponse
    if (errors.length > 0) {
      return { message: "Certaines trames ont été ignorées.", details: errors };
    } else {
      return { message: "Toutes les trames CAN ont été traitées et insérées avec succès !" };
    }

  } catch (error) {
    console.error("Erreur lors de l'insertion des trames CAN :", error);
    return { error: "Erreur serveur" };
  }
};
// exports.PostTrame = async (body) => { // On récupère directement `body`
//   try {
//     const { users, data } = body; // Extraction des données

//     // Vérification du format JSON
//     if (!users || !Array.isArray(data)) {
//       return { error: "Format JSON invalide" };
//     }

//     for (const trame of data) {
//       let { idCAN, lenData, Data } = trame;

//       if (!idCAN || !lenData || !Data || Data.toString().length !== 16) {
//         return { error: "Données CAN invalides" };
//       }

//        //////////////Il faut retirer les 0x avant chaque caractère HEXA//////////////////

//       idCAN = idCAN.toUpperCase();
//       const octets = Data.toString().match(/.{2}/g);

//       // Vérifier si l'identifiant existe déjà dans `identifiants_can`
//       const checkQuery = `SELECT COUNT(*) FROM identifiants_can WHERE can_id = $1`;
//       const checkResult = await db.query(checkQuery, [idCAN]);

//       if (checkResult.rows[0].count == 0) {
//         // Ajouter l'identifiant s'il n'existe pas
//         const insertIdentifiantQuery = `INSERT INTO identifiants_can (can_id) VALUES ($1)`;
//         await db.query(insertIdentifiantQuery, [idCAN]);
//       }

//       // Insérer la trame dans `trames_can`
//       const insertTrameQuery = `
//         INSERT INTO trames_can (can_id, horodatage, octet_0, octet_1, octet_2, octet_3, octet_4, octet_5, octet_6, octet_7)
//         VALUES ($1, NOW(), $2, $3, $4, $5, $6, $7, $8, $9)`;

//       await db.query(insertTrameQuery, [idCAN, ...octets]);
//     }

//     return { message: "Trames CAN insérées avec succès !" };

//   } catch (error) {
//     console.error("Erreur lors de l'insertion des trames CAN :", error);
//     return { error: "Erreur serveur" };
//   }
// };
// exports.PostTrame = async (req, res) => {
//   try {
//     //console.log("Requête reçue :", req.body); // Ajoute ce log
//     //console.log(req);
//     const data = req.data;

//     if (!Array.isArray(data)) {
//       return res.status(400).json({ error: "Format JSON invalide" });
//     }
//    // console.log("testoui");
//     for (const trame of data) {
//      // console.log("debut");
//      // console.log(trame);
//       let { idCAN, lenData, Data } = trame;

//       if (!idCAN || !lenData || !Data || Data.toString().length !== 16) {
//         return res.status(400).json({ error: "Données CAN invalides" });
//       }

//       idCAN = idCAN.toUpperCase();
//       const octets = Data.toString().match(/.{2}/g);
//        // console.log("envoiereq");
//       const query = `
//         INSERT INTO trames_can (can_id, horodatage, octet_0, octet_1, octet_2, octet_3, octet_4, octet_5, octet_6, octet_7)
//         VALUES ($1, NOW(), $2, $3, $4, $5, $6, $7, $8, $9)
//       `;

//       await db.query(query, [idCAN, ...octets]);
//       //console.log("requetefinit");
//     }

//     return(  "Trames CAN insérées avec succès !" );

//   } catch (error) {
//     console.error("Erreur lors de l'insertion des trames CAN :", error);
//     res.status(500).json({ error: "Erreur serveur" });
//   }
// };
// Récupérer un item par ID
// exports.getById = async (id) => {
//   const [rows] = await db.promise().query('SELECT * FROM  WHERE id = ?', [id]);
//   return rows[0];
// };

// // Ajouter un nouvel item
// exports.add = async (newItem) => {
//   const [result] = await db.promise().query('INSERT INTO items (name, type) VALUES (?, ?)', [
//     newItem.name,
//     newItem.type,
//   ]);
//   return { id: result.insertId, ...newItem };
// };

// // Mettre à jour un item
// exports.update = async (id, updatedItem) => {
//   const [result] = await db.promise().query(
//     'UPDATE items SET name = ?, type = ? WHERE id = ?',
//     [updatedItem.name, updatedItem.type, id]
//   );
//   if (result.affectedRows > 0) {
//     return { id, ...updatedItem };
//   }
//   return null;
// };

// // Supprimer un item
// exports.delete = async (id) => {
//   const [result] = await db.promise().query('DELETE FROM items WHERE id = ?', [id]);
//   return result.affectedRows > 0;
// };