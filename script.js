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
                document.getElementById('apropos').classList.remove('active')
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


