var bPedir = document.getElementById('pedir')
bPedir.addEventListener('click', function() {
    axios.post ('http://ea49304ce336.ngrok.io/hola', 
    {
        nomb : document.getElementById('nomb').value
    }).then (function(res){
        console.log(res.data)
    }).catch(function(err){
        console.log(err)
    })
})