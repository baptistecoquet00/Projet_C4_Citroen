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
    const query = "SELECT * FROM significations_can WHERE can_id = $1";
    const result = await db.query(query, [id]); // Pas de conversion, id reste une chaîne
    console.log(result.rows);
    return result.rows;
  } catch (error) {
    console.error("Erreur lors de la récupération des significations :", error);
    throw error;
  }
};
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
