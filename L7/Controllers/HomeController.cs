using Microsoft.AspNetCore.Mvc;

namespace Graphics.Controllers
{
    public class HomeController : Controller
    {
        public IActionResult Index()
        {
            return View();
        }
    }
}