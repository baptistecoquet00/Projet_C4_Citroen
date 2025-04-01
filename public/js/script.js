document.getElementById('identifiants').addEventListener('click',AfficherIdentifiants);

document.addEventListener('DOMContentLoaded', () => {
    const menuLinks = document.querySelectorAll('.menu a');
    const sections = document.querySelectorAll('main section');

    // Gestion de la navigation
    menuLinks.forEach(link => {
        link.addEventListener('click', (e) => {
            e.preventDefault();
            const targetId = link.getAttribute('href').substring(1);

            // Si le lien "Accueil" est cliqué
            if (targetId === 'accueil') {
                // Afficher les deux sections "section1" et "section2" 
                document.getElementById('section1').classList.add('active');
                document.getElementById('section2').classList.add('active');
                document.getElementById('apropos').classList.remove('active');
                document.getElementById('iidentifiants').classList.remove('active');
            } else {
                // Pour les autres liens, on masque toutes les sections
                sections.forEach(section => {
                    section.classList.remove('active');
                });
                // Afficher la section liée au lien cliqué
                const targetSection = document.getElementById(targetId);
                if (targetSection) {
                    targetSection.classList.add('active');
                }
            }
        });
    });
});


function AffichierIdentifiantsEtSignifications() 
{
    const xhttp = new XMLHttpRequest();
    xhttp.open('GET','http://172.18.110.111:3000/api/significations');
    xhttp.onreadystatechange = function()
    {
    if (this.readyState == 4 && this.status == 200) {
     var reponse='';   
    var reponse=this.responseText;
    const json = reponse ;
    reponse = JSON.parse(json);
    console.log(reponse); 
    
  var tableau="<table id='table'><tr><th>Id Can</th><th>Octet</th><th>Significations</th></tr>";
    for(let i=0; i<reponse.length;i++)
        {
           tableau=tableau+"<tr><td>"+reponse[i]['can_id']+"</td><td>"+reponse[i]['octet']+"</td><td>"+reponse[i]['signification']+"</td></tr>"
           
        }
    tableau=tableau+"</table>";
    document.getElementById('tableau').innerHTML=tableau;
        }  
    }
    xhttp.send();
}


function RecupSignifications(canId) {
    const xhttp = new XMLHttpRequest();
    xhttp.open('GET', `http://172.18.110.111:3000/api/significations/${canId}`);
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) 
            {
            var reponse = JSON.parse(this.responseText);
            console.log(reponse);
            var detailsDiv = document.getElementById(`details-${canId}`);
            // Vérifier si le tableau est déjà affiché
            if (detailsDiv.innerHTML !== "") 
                {
                    detailsDiv.innerHTML = ""; // Cache le tableau s'il est déjà affiché
            } else 
                {
                    var tableau = `<table class="sub-table"><tr><th>Octet</th><th>Significations</th></tr>`;
                    for (let i = 0; i < reponse.length; i++) 
                    {
                        tableau += `<tr><td>${reponse[i]['octet']}</td><td>${reponse[i]['signification']}</td></tr>`;
                    }
                    tableau += "</table>";
                    detailsDiv.innerHTML = tableau;
                }
            }
    };
    xhttp.send();
}

function detecterEntree(event) {
    if (event.key === "Enter") {
        AfficherIdentifiants();
    }
}

function AfficherIdentifiants() {
    let IdRecherche = document.getElementById('RechercheUnCan').value.trim(); // Récupère et nettoie l'input
    let url = "http://172.18.110.111:3000/api/identifiants";
    if (IdRecherche) 
        {
        url += `/${IdRecherche}`;
        }
    const xhttp = new XMLHttpRequest();
    xhttp.open('GET', url);
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var reponse = JSON.parse(this.responseText);
            console.log(reponse);
            var tableau = "<table id='table'><tr><th>Id Can</th></tr>";
            for (let i = 0; i < reponse.length; i++) {
                tableau += `<tr><td><a href="#" class="can-lien" data-id="${reponse[i]['can_id']}">${reponse[i]['can_id']}</a><div class="hidden" id="details-${reponse[i]['can_id']}"></div></td></tr>`;
            }
            tableau += "</table>";
            document.getElementById('tableau').innerHTML = tableau;

            // Ajouter l'événement de clic pour récupérer les significations
            document.querySelectorAll('.can-lien').forEach(link => {
                link.addEventListener('click', function (event) {
                    event.preventDefault();
                    let canId = this.getAttribute('data-id');
                    RecupSignifications(canId);
                });
            });
        }
    };
    xhttp.send();
}