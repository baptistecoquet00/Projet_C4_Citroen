const express = require('express');
const router = express.Router();
const apiController = require('../../controllers/apiController');

//router.get('/data', apiController.getData);
router.get('/identifiants', apiController.getIdentifiants);// GET tous les identifiants
router.get('/identifiants/:id', apiController.getUnIdentifiant);

router.get('/significations', apiController.getSignificationsTest);
router.get('/significations/:id', apiController.getSignifications);
router.get('/significations/:id/:octet', apiController.getSignificationsUnOctet);

router.post('/trame',apiController.PostTrame);
router.get('/trame/:id', apiController.getTramesParId);

router.get('/lien', apiController.lien);

router.get('/regime/:id',apiController.getRegime);
module.exports = router;

//172.18.110.111:3000/api/