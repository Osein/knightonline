using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace UIEditor.EngineRenderSurface
{
    /// <summary>
    /// Interaction logic for EngineRenderSurface.xaml
    /// </summary>
    public partial class EngineRenderSurface : UserControl, IDisposable
    {
        EngineRenderSurfaceHost _host;

        public EngineRenderSurface()
        {
            InitializeComponent();
            Loaded += OnRenderSurfaceLoaded;
        }

        public void Dispose()
        {
            //throw new NotImplementedException();
            _host.Dispose();
        }

        private void OnRenderSurfaceLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnRenderSurfaceLoaded;

            _host = new EngineRenderSurfaceHost();

            Content = _host;
        }
    }
}
