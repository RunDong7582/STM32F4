//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.261
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace KEES_ADC_SAR_v2_0 {
    using System;
    
    
    /// <summary>
    ///   A strongly-typed resource class, for looking up localized strings, etc.
    /// </summary>
    // This class was auto-generated by the StronglyTypedResourceBuilder
    // class via a tool like ResGen or Visual Studio.
    // To add or remove a member, edit your .ResX file then rerun ResGen
    // with the /str option, or rebuild your VS project.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "4.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    internal class Resources {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Resources() {
        }
        
        /// <summary>
        ///   Returns the cached ResourceManager instance used by this class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("KEES_ADC_SAR_v2_0.Resources", typeof(Resources).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   Overrides the current thread's CurrentUICulture property for all
        ///   resource lookups using this strongly typed resource class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Built-in.
        /// </summary>
        internal static string BuiltInTabCaption {
            get {
                return ResourceManager.GetString("BuiltInTabCaption", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Clock frequency value must be between {0} MHz and {1} MHz..
        /// </summary>
        internal static string ClockFrequencyErrorMsg {
            get {
                return ResourceManager.GetString("ClockFrequencyErrorMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Configure.
        /// </summary>
        internal static string ConfigureTabCaption {
            get {
                return ResourceManager.GetString("ConfigureTabCaption", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Conversion Rate value must be between {0} Sps and {1} Sps..
        /// </summary>
        internal static string ConversionRateErrorMsg {
            get {
                return ResourceManager.GetString("ConversionRateErrorMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Internal clock frequency must be between {0} MHz and {1} MHz. Please change ADC Resolution or Conversion Rate..
        /// </summary>
        internal static string DRCConversionRate {
            get {
                return ResourceManager.GetString("DRCConversionRate", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Internal clock frequency value must be between {0} MHz + {1}% ({2} MHz) and {3} MHz - {4}% ({5} MHz)..
        /// </summary>
        internal static string DRCInternalClockFreqMsg {
            get {
                return ResourceManager.GetString("DRCInternalClockFreqMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to The internal clock minimum pulse width must be greater than {0} ns..
        /// </summary>
        internal static string DRCMinimumPulseWidthMsg {
            get {
                return ResourceManager.GetString("DRCMinimumPulseWidthMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to External clock is undefined or was changed. For proper component operation, launch the SAR ADC configuration dialog and select the appropriate value for Conversion Rate or Clock Source..
        /// </summary>
        internal static string InvalidExternalClock {
            get {
                return ResourceManager.GetString("InvalidExternalClock", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Conversion rate for &quot;Internal Vref&quot; Reference must be less than 100ksps. Please use &quot;Internal Vref, bypassed&quot; for higher rate..
        /// </summary>
        internal static string InvalidReferenceErrorMsg {
            get {
                return ResourceManager.GetString("InvalidReferenceErrorMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to EOS output is not available for PSoC 5 device..
        /// </summary>
        internal static string NextOutErrorMsg {
            get {
                return ResourceManager.GetString("NextOutErrorMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to SAR ADC Reference must be internal when Input Range is &quot;{0}&quot;.
        /// </summary>
        internal static string VDACExternalRefErrorMsg {
            get {
                return ResourceManager.GetString("VDACExternalRefErrorMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to The voltage reference of SAR ADC should be less than system VDDA voltage..
        /// </summary>
        internal static string VddaDRCMsg {
            get {
                return ResourceManager.GetString("VddaDRCMsg", resourceCulture);
            }
        }

        /// <summary>
        ///   Looks up a localized string similar to SAR ADC Reference cannot be set as &quot;Internal Vref, bypassed&quot; when Input Range is &quot;{0}&quot;..
        /// </summary>
        internal static string VddaInternalBypassErrorMsg {
            get {
                return ResourceManager.GetString("VddaInternalBypassErrorMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Voltage Reference value must be between {0} V and Vdda ({1} V)..
        /// </summary>
        internal static string VoltageReferenceErrorMsg {
            get {
                return ResourceManager.GetString("VoltageReferenceErrorMsg", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Voltage Reference value must be between {0} V and Vdda/2 ({1} V)..
        /// </summary>
        internal static string VoltageReferenceErrorMsgVddaDiv2 {
            get {
                return ResourceManager.GetString("VoltageReferenceErrorMsgVddaDiv2", resourceCulture);
            }
        }
    }
}
