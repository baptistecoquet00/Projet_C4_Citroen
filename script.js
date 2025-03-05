document.addEventListener('DOMContentLoaded', () => {
    const menuLinks = document.querySelectorAll('.menu a');
    const sections = document.querySelectorAll('main section');

    // Gestion de la navigation
    menuLinks.forEach(link => {
        link.addEventListener('click', (e) => {
            e.preventDefault();
            const targetId = link.getAttribute('href').substring(1);

            // Masquer toutes les sections
            sections.forEach(section => {
                section.classList.remove('active');
            });

            // Afficher la section ciblée
            document.getElementById(targetId).classList.add('active');
        });
    });

});
